import torch
import cv2
import serial
import time

arduino = serial.Serial(
    port='COM3',    # CHANGE this (check Device Manager)
    baudrate=115200,
    timeout=1
)

time.sleep(2)  # wait for Arduino reset


# Load YOLOv5 nano model
model = torch.hub.load(
    'ultralytics/yolov5',
    'yolov5n',
    pretrained=True
)

# Set confidence threshold
model.conf = 0.45

# Bird class ID in COCO
BIRD_CLASS_ID = 14

# Open webcam
cap = cv2.VideoCapture(1)

# ---- Motion detection setup ----
prev_gray = None
MOTION_AREA_THRESHOLD = 1500   # tune later
MOTION_FRAMES_REQUIRED = 2
motion_counter = 0

#bird mapping
PIXELS_TO_STEPS = 0.08   # tune later
AIM_DEADZONE = 20        # pixels


print("Smart Scarecrow running with motion gating. Press Q to quit.")

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.GaussianBlur(gray, (21, 21), 0)

    motion_detected = False

    if prev_gray is None:
        prev_gray = gray
        continue

    # Frame difference
    frame_diff = cv2.absdiff(prev_gray, gray)
    thresh = cv2.threshold(frame_diff, 25, 255, cv2.THRESH_BINARY)[1]
    thresh = cv2.dilate(thresh, None, iterations=2)

    contours, _ = cv2.findContours(
        thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
    )

    for cnt in contours:
        if cv2.contourArea(cnt) > MOTION_AREA_THRESHOLD:
            motion_detected = True
            break

    if motion_detected:
        motion_counter += 1
    else:
        motion_counter = 0

    prev_gray = gray

    # ---- Run YOLO only if motion persists ----
    if motion_counter >= MOTION_FRAMES_REQUIRED:
        results = model(frame)
        detections = results.xyxy[0]

        for *box, conf, cls in detections:
            if int(cls) == BIRD_CLASS_ID:
                x1, y1, x2, y2 = map(int, box)
                cx = int((x1 + x2) / 2)
                cy = int((y1 + y2) / 2)

                frame_h, frame_w = frame.shape[:2]
                error_x = cx - (frame_w // 2)

                if abs(error_x) > AIM_DEADZONE:
                    step_command = int(error_x * PIXELS_TO_STEPS)
                    command = f"MOVE {step_command}\n"
                    arduino.write(command.encode())
                else:
                    print("Target aligned")


                print(f"Bird detected | center=({cx},{cy}) | x1,y1=({x1},{y1}) | x2,y2=({x2},{y2})")

                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)
                cv2.putText(
                    frame,
                    f"Bird {conf:.2f}",
                    (x1, y1 - 10),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.6,
                    (0, 255, 0),
                    2
                )

        motion_counter = 0  # reset after YOLO

    cv2.imshow("Smart Scarecrow (Motion-Gated)", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
