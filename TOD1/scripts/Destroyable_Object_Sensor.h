class Destroyable_Object_Sensor : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Destroyable_Object_Sensor", "Node", false, false);
		
		//	TODO: add members and methods to Destroyable_Object_Sensor.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Destroyable_Object_Sensor::Create();