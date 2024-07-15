class Master_DelType : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Master_DelType", "Node", false, false);
		
		//	TODO: add members and methods to Master_DelType.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Master_DelType::Create();