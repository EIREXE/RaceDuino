import PyCmdMessenger
import time


arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=9600)

# List of command names (and formats for their associated arguments). These must
# be in the same order as in the sketch.
commands = [["handshake", "s"],
	   ["change_gear", "i"],
	   ["change_rpm", "i"],
	   ["change_ers", "f"],]

# Initialize the messenger
c = PyCmdMessenger.CmdMessenger(arduino,commands)

# Send
c.send("handshake")
c.send("change_ers", 0.5)
time.sleep(0.25)
c.send("change_ers", 0.75)
c.send("change_rpm", 5000)
time.sleep(0.25)
c.send("change_rpm", 100)
c.send("change_ers", 0)
# Receive. Should give ["my_name_is",["Bob"],TIME_RECIEVED]
msg = c.receive()
print(msg)
