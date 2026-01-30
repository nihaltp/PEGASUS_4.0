import serial
import time

arduino = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    timeout=0.1   # short timeout keeps loop responsive
)

time.sleep(2)  # Arduino reset

print("Enter stepper to move with steps")
print("Format: <stepper> <steps>")
print("Example: laser 100")
print("Type 'exit' to quit")

while True:
    command = input("> ").strip()

    if command.lower() == "exit":
        print("Closing serial...")
        arduino.close()
        break

    # send command
    arduino.write((command + "\n").encode("utf-8"))

    # read responses for a short window
    start_time = time.time()
    while time.time() - start_time < 0.5:
        if arduino.in_waiting > 0:
            line = arduino.readline().decode("utf-8").strip()
            if line:
                print("Arduino:", line)
