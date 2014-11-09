extends RigidBody2D

var thrust = 1000
var camber = 5
var angle_ratio = 16
var stall_angle = 15
var rho = 1.002
var wing_area = 5
var wing_offset = 20
var drag_offset = 100
var elevator_offset = 100

func _ready():
	set_use_custom_integrator(true)

func cLift(angle):
	return (min(max(-angle, -stall_angle), stall_angle )+ camber) / angle_ratio

func cDrag(angle):
	return 100

func _integrate_forces(state):
	var laccel = state.get_total_gravity()
	var aaccel = 0
	var front = Vector2(1, 0).rotated(get_rot())
	var top = Vector2(0, -1).rotated(get_rot())
	var air_vel = state.get_linear_velocity()
	var wing_speed = air_vel.dot(front)
	var incline = rad2deg(front.angle_to(air_vel))
	# Thrust
	if (Input.is_action_pressed("thrust")):
		laccel += front * thrust
	# Drag
	var drag = air_vel * air_vel.length() * 0.005
	laccel -= drag * state.get_inverse_mass()
	aaccel -= drag.dot(top) * drag_offset * state.get_inverse_inertia()
	# Lift
	var wing_lift = wing_speed * wing_speed * cLift(incline) * 0.003
	laccel += top * wing_lift
	aaccel += wing_lift * wing_offset * state.get_inverse_inertia()
	# Elevator Lift
	var elevator_lift = wing_speed * wing_speed * 0.003
	if (Input.is_action_pressed("elevator_up")):
		aaccel += elevator_lift * elevator_offset * state.get_inverse_inertia()
	if (Input.is_action_pressed("elevator_down")):
		aaccel -= elevator_lift * elevator_offset * state.get_inverse_inertia()
	get_node("label").set_text(str(wing_lift))
	state.set_linear_velocity(state.get_linear_velocity() + laccel * state.get_step())
	state.set_angular_velocity(state.get_angular_velocity() + aaccel * state.get_step())
