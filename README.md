# RF climate sensor (BME280)

Climate sensor on ATmega328 + RF24 + BME280.

## Features

1. Power consumption in sleep mode approximately 5 μA. Up to 3 years of operation on one CR2450 battery (estimated).
2. Transmits battery level charge, humidity, temperature and air pressure every 5 min (can be changed up to 65535 seconds).
3. Automatic restart in case of a hang-up.

## Note

A gateway is required. For details see "RF Gateway".

## Full config example for Home Assistant

```yml
mqtt:
  sensor:
    - name: "NAME"
      device_class: "voltage"
      unit_of_measurement: "V"
      state_topic: "homeassistant/rf_sensor/ID/bme280"
      value_template: "{{ value_json.battery }}"
      expire_after: 375
      force_update: true
      qos: 2
    - name: "NAME"
      device_class: "humidity"
      unit_of_measurement: "%"
      state_topic: "homeassistant/rf_sensor/ID/bme280"
      value_template: "{{ value_json.humidity }}"
      expire_after: 375
      force_update: true
      qos: 2
    - name: "NAME"
      device_class: "temperature"
      unit_of_measurement: "°C"
      state_topic: "homeassistant/rf_sensor/ID/bme280"
      value_template: "{{ value_json.temperature }}"
      expire_after: 375
      force_update: true
      qos: 2
    - name: "NAME"
      evice_class: "pressure"
      unit_of_measurement: "мм"
      state_topic: "homeassistant/rf_sensor/ID/bme280"
      value_template: "{{ value_json.pressure }}"
      expire_after: 375
      force_update: true
      qos: 2
```
