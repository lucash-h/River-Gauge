# River-Gauge
An attempt at creating self sufficient river gauge using an MKR GSM 1400 Arduino Board and an ultrasonic sensor. When a specific message is sent to the Arduino board (currently "GIVEMEVALUE"), the river value will be calculated using an average of, currently 50 ultrasonic sensor readings, then send back that value to the sender in another text message.
