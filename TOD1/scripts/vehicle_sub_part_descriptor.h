class vehicle_sub_part_descriptor : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vehicle_sub_part_descriptor", "Node", false, false);
		
		//	TODO: add members and methods to vehicle_sub_part_descriptor.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vehicle_sub_part_descriptor::Create();