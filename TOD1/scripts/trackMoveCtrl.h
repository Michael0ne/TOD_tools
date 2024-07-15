class trackMoveCtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("trackMoveCtrl", "Node", false, false);
		
		//	TODO: add members and methods to trackMoveCtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

trackMoveCtrl::Create();