class car_collide : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("car_collide", "Node", false, false);
		
		//	TODO: add members and methods to car_collide.
		
		inst->CalculateSize();
		
		return inst;
	};
};

car_collide::Create();