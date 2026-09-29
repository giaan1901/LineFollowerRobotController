#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

/* Confirm before wiring: five I2C addresses and bus, HX711 pins, motor
 * driver and pins, encoder availability, sensor spacing, and load calibration.
 * No assignments are supplied here because none has been verified. */
typedef struct {
    float sensor_pitch_m;       /* 0 until measured; adjacent sensor spacing */
    float full_command_mps;     /* 0 until speed-to-command calibration */
    float load_scale_kg_per_count; /* 0 until HX711 calibration */
} HardwareCalibration;

#endif
