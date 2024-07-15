class interest_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("interest_point", "Node", false, false);
		
		//	TODO: add members and methods to interest_point.
		
		inst->CalculateSize();
		
		return inst;
	};
};

interest_point::Create();