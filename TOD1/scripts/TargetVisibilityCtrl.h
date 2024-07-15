class TargetVisibilityCtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("TargetVisibilityCtrl", "Node", false, false);
		
		//	TODO: add members and methods to TargetVisibilityCtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

TargetVisibilityCtrl::Create();