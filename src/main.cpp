#include "Arduino.h"
#include "Arduino_FreeRTOS.h"
#include "Adafruit_BME280.h"
#include "RF24.h"

typedef struct
{
  int16_t sensor_id{0};
  int16_t sensor_type{0};
  int16_t value_1{0};
  int16_t value_2{0};
  int16_t value_3{0};
  int16_t value_4{0};
  int16_t value_5{0};
} transmitted_data_t;

void sendSensorsValue(void *pvParameters);
float getBatteryLevelCharge(void);

RF24 radio(9, 10);
Adafruit_BME280 bme;

void setup()
{
  ADCSRA &= ~(1 << ADEN);
  radio.begin();
  radio.setChannel(120);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(14);
  radio.setAddressWidth(3);
  radio.setCRCLength(RF24_CRC_8);
  radio.setRetries(15, 15);
  radio.openWritingPipe(0xDDEEFF);
  radio.powerDown();
  bme.begin(0x76);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X16,
                  Adafruit_BME280::SAMPLING_X16,
                  Adafruit_BME280::SAMPLING_X16,
                  Adafruit_BME280::FILTER_X16);
  xTaskCreate(sendSensorsValue, "Send Sensors Value", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void loop()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  portENTER_CRITICAL();
  sleep_enable();
  portEXIT_CRITICAL();
  sleep_cpu();
  sleep_reset();
}

void sendSensorsValue(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    transmitted_data_t sensor{abs((int16_t)ID), BME280};
    bme.takeForcedMeasurement();
    sensor.value_1 = getBatteryLevelCharge() * 100;
    sensor.value_2 = bme.readHumidity();
    sensor.value_3 = bme.readTemperature();
    sensor.value_4 = bme.readPressure() * 0.00750062;
    radio.powerUp();
    radio.flush_tx();
    radio.write(&sensor, sizeof(transmitted_data_t));
    radio.powerDown();
    vTaskDelay(300);
  }
  vTaskDelete(NULL);
}

float getBatteryLevelCharge()
{
  ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
  ADCSRA |= (1 << ADEN);
  delay(10);
  ADCSRA |= (1 << ADSC);
  while (bit_is_set(ADCSRA, ADSC))
    ;
  ADCSRA &= ~(1 << ADEN);
  float value = ((1024 * 1.1) / (ADCL + ADCH * 256));
  return value;
}