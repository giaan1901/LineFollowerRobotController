#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

/* Assignment requirements. All distances are metres and speeds are m/s. */
#define ROBOT_DESIGN_SPEED_MPS 0.30f
#define ROBOT_MIN_SPEED_MPS 0.10f
#define ROBOT_LINE_WIDTH_M 0.026f
#define ROBOT_LINE_EDGE_TOLERANCE_M 0.003f
#define ROBOT_STOP_TOLERANCE_M 0.005f
#define ROBOT_MIN_CURVE_RADIUS_M 0.50f
#define ROBOT_MAX_PAYLOAD_KG 2.0f
#define ROBOT_SENSOR_COUNT 5u

/* Initial software choices; tune on the assembled robot. */
#define ROBOT_DEFAULT_KP 0.5f
#define ROBOT_DEFAULT_KI 0.0f
#define ROBOT_DEFAULT_KD 0.0f
#define ROBOT_DEFAULT_STEERING_LIMIT 1.0f
#define ROBOT_LOAD_1KG_MIN_KG 0.8f
#define ROBOT_LOAD_1KG_MAX_KG 1.2f
#define ROBOT_LOAD_2KG_MIN_KG 1.8f
#define ROBOT_LOAD_2KG_MAX_KG 2.0f

#endif
