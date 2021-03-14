/* Play music to from local server

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "tcp_client_stream.h"
#include "opus_decoder.h"
#include "opus_encoder.h"
#include "wav_encoder.h"
#include "i2s_stream.h"

#include "esp_peripherals.h"
#include "periph_wifi.h"
#include "periph_sdcard.h"
#include "board.h"

static const char *TAG = "TCP_CLIENT_EXAMPLE";

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    tcpip_adapter_init();

    audio_pipeline_handle_t pipeline, pipeline_out;
    audio_element_handle_t tcp_stream_reader,tcp_stream_writter,wav_encoder, opus_decoder,opus_encoder, i2s_stream_writer,i2s_stream_reader;

    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set(TAG, ESP_LOG_DEBUG);

    ESP_LOGI(TAG, "[ 1 ] Start codec chip");
    audio_board_handle_t board_handle = audio_board_init();
    audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_DECODE, AUDIO_HAL_CTRL_START);

    ESP_LOGI(TAG, "[2.0] Create audio pipeline for playback");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    pipeline = audio_pipeline_init(&pipeline_cfg);
    pipeline_out = audio_pipeline_init(&pipeline_cfg);
    AUDIO_NULL_CHECK(TAG, pipeline, return);

    ESP_LOGI(TAG, "[2.1] Create i2s stream to write data to codec chip");
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_cfg.i2s_config.sample_rate = 16000;
    i2s_cfg.i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg);
    AUDIO_NULL_CHECK(TAG, i2s_stream_writer, return);

    i2s_cfg.type = AUDIO_STREAM_READER;
    i2s_stream_reader = i2s_stream_init(&i2s_cfg);

    ESP_LOGI(TAG, "[2.2] Create wav decoder to decode wav file");
    opus_decoder_cfg_t opus_cfg = DEFAULT_OPUS_DECODER_CONFIG();
    opus_cfg.task_core = 1; // if use core 0 music will be disfluency
    opus_cfg.task_prio = 30;
    opus_decoder = decoder_opus_init(&opus_cfg);
    AUDIO_NULL_CHECK(TAG, opus_decoder, return);

    opus_encoder_cfg_t opus_cfg_e = DEFAULT_OPUS_ENCODER_CONFIG();
    opus_cfg_e.sample_rate        = 16000;
    opus_cfg_e.channel            = 1;
    opus_cfg_e.bitrate            = 64000;
    opus_cfg_e.complexity         = 0;
    opus_cfg_e.task_core = 0; // if use core 0 music will be disfluency
    opus_cfg_e.task_prio = 30;
    opus_encoder = encoder_opus_init(&opus_cfg_e);
    AUDIO_NULL_CHECK(TAG, opus_encoder, return);

    wav_encoder_cfg_t wav_cfg_e = DEFAULT_WAV_ENCODER_CONFIG();
    wav_encoder = wav_encoder_init(&wav_cfg_e);

    ESP_LOGI(TAG, "[2.2] Create tcp client stream to read data");
    tcp_stream_cfg_t tcp_cfg = TCP_STREAM_CFG_DEFAULT();
    // tcp_cfg.type = AUDIO_STREAM_READER;
    // tcp_cfg.port = CONFIG_TCP_PORT;
    // tcp_cfg.host = CONFIG_TCP_URL;
    // tcp_stream_reader = tcp_stream_init(&tcp_cfg);
    // AUDIO_NULL_CHECK(TAG, tcp_stream_reader, return);

    tcp_cfg.type = AUDIO_STREAM_WRITER;
    tcp_cfg.port = CONFIG_TCP_PORT_OUT;
    tcp_cfg.host = CONFIG_TCP_URL;
    tcp_stream_writter = tcp_stream_init(&tcp_cfg);
    AUDIO_NULL_CHECK(TAG, tcp_stream_writter, return);


    ESP_LOGI(TAG, "[2.3] Register all elements to audio pipeline");
    // audio_pipeline_register(pipeline, tcp_stream_reader, "tcp");
    // audio_pipeline_register(pipeline, opus_decoder, "opus");
    // audio_pipeline_register(pipeline, i2s_stream_writer, "i2s");

    audio_pipeline_register(pipeline_out, tcp_stream_writter, "tcp_w");
    // audio_pipeline_register(pipeline_out, opus_decoder, "opus_d");
    // audio_pipeline_register(pipeline_out, opus_encoder, "opus_e");
    audio_pipeline_register(pipeline_out,wav_encoder,"wav_e");
    // audio_pipeline_register(pipeline_out, i2s_stream_writer, "i2s_w");
    audio_pipeline_register(pipeline_out, i2s_stream_reader, "i2s_r");

    ESP_LOGI(TAG, "[2.4] Link it together tcp-->wav-->i2s");
    // audio_pipeline_link(pipeline, (const char *[]) {"tcp", "opus", "i2s"}, 3);
    audio_pipeline_link(pipeline_out, (const char *[]) {"i2s_r","wav_e", "tcp_w"}, 3);

    ESP_LOGI(TAG, "[ 3 ] Start and wait for Wi-Fi network");
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);
    periph_wifi_cfg_t wifi_cfg = {
        .ssid = CONFIG_WIFI_SSID,
        .password = CONFIG_WIFI_PASSWORD,
    };
    esp_periph_handle_t wifi_handle = periph_wifi_init(&wifi_cfg);
    esp_periph_start(set, wifi_handle);
    periph_wifi_wait_for_connected(wifi_handle, portMAX_DELAY);

    ESP_LOGI(TAG, "[ 4 ] Set up  event listener");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

    ESP_LOGI(TAG, "[4.1] Listening event from all elements of pipeline");
    // audio_pipeline_set_listener(pipeline, evt);
    audio_pipeline_set_listener(pipeline_out, evt);

    ESP_LOGI(TAG, "[4.2] Listening event from peripherals");
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    ESP_LOGI(TAG, "[ 5 ] Start audio_pipeline");
    // audio_pipeline_run(pipeline);
    audio_pipeline_run(pipeline_out);

    while (1) {
        audio_event_iface_msg_t msg;
        esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[ * ] Event interface error : %d", ret);
            continue;
        }

        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) opus_decoder
            && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
            audio_element_info_t music_info = {0};
            audio_element_getinfo(opus_decoder, &music_info);

            ESP_LOGI(TAG, "[ * ] Receive music info from mp3 decoder, sample_rates=%d, bits=%d, ch=%d",
                     music_info.sample_rates, music_info.bits, music_info.channels);

            audio_element_setinfo(i2s_stream_writer, &music_info);
            i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
            continue;
        }

        /* Stop when the last pipeline element (i2s_stream_writer in this case) receives stop event */
        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) i2s_stream_writer
            && msg.cmd == AEL_MSG_CMD_REPORT_STATUS
            && (((int)msg.data == AEL_STATUS_STATE_STOPPED) || ((int)msg.data == AEL_STATUS_STATE_FINISHED))) {
            ESP_LOGW(TAG, "[ * ] Stop event received");
            break;
        }
    }

    ESP_LOGI(TAG, "[ 6 ] Stop audio_pipeline");
    audio_pipeline_stop(pipeline);
    audio_pipeline_wait_for_stop(pipeline);
    audio_pipeline_terminate(pipeline);

    /* Terminate the pipeline before removing the listener */
    // audio_pipeline_unregister(pipeline, tcp_stream_reader);
    audio_pipeline_unregister(pipeline, i2s_stream_writer);
    audio_pipeline_unregister(pipeline, opus_decoder);

    audio_pipeline_remove_listener(pipeline);

    /* Stop all peripherals before removing the listener */
    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

    /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
    audio_event_iface_destroy(evt);

    /* Release all resources */
    audio_pipeline_deinit(pipeline);
    // audio_element_deinit(tcp_stream_reader);
    audio_element_deinit(i2s_stream_writer);
    audio_element_deinit(opus_decoder);
    esp_periph_set_destroy(set);
}
