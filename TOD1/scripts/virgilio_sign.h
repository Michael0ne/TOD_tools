class virgilio_sign : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("virgilio_sign", "Node", false, false);
		
		//	TODO: add members and methods to virgilio_sign.
		
		inst->CalculateSize();
		
		return inst;
	};
};

virgilio_sign::Create();