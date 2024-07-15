class Vehicle_sound_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Vehicle_sound_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to Vehicle_sound_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Vehicle_sound_ctrl::Create();