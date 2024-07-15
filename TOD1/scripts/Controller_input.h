class Controller_input : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Controller_input", "Node", false, false);
		
		//	TODO: add members and methods to Controller_input.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Controller_input::Create();