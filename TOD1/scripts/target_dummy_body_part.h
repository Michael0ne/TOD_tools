class target_dummy_body_part : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("target_dummy_body_part", "Node", false, false);
		
		//	TODO: add members and methods to target_dummy_body_part.
		
		inst->CalculateSize();
		
		return inst;
	};
};

target_dummy_body_part::Create();