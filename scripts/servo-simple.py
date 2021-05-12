import rospy
from std_msgs.msg import String #Connection
import time

serial_connection = "global"

def callback(data):
	print(rospy.get_caller_id() + 'I heard %s', data.data)
	servo_value=data.data.split(",")
	dyn_id=int(servo_value[0])
	goal_location = int(servo_value[1])
	print("id: " + servo_value[0] + " position " + servo_value[1])
	#serial_connection.goto(dyn_id, goal_location, speed=512, degrees=True)

def listener():
	
	rospy.init_node('servo', anonymous=True)
	
	rospy.Subscriber('servo_control', String, callback)
	
	# spin() simply keeps python fom exciting until this node is stopped
	rospy.spin()

if __name__=='__main__':
	# Connect to the serial port
	#serial_connection = Connection(port="/dev/ttyACM0", baudrate=57600)
	#dynamixel_id = 5
	#is_available=serial_connection.ping(dynamixel_id)
	print('Servo is ready')
	print('Servo Node Opened')
	listener()
	#serial_connection.close()