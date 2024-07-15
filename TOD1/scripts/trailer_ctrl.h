class trailer_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("trailer_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to trailer_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

trailer_ctrl::Create();