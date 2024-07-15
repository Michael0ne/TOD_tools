class FOG_Controller : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("FOG_Controller", "Node", false, false);
		
		//	TODO: add members and methods to FOG_Controller.
		
		inst->CalculateSize();
		
		return inst;
	};
};

FOG_Controller::Create();