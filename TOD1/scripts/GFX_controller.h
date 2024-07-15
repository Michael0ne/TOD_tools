class GFX_controller : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("GFX_controller", "Node", false, false);
		
		//	TODO: add members and methods to GFX_controller.
		
		inst->CalculateSize();
		
		return inst;
	};
};

GFX_controller::Create();