class car_move_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("car_move_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to car_move_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

car_move_ctrl::Create();