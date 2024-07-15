class Gotopath_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Gotopath_point", "Node", false, false);
		
		//	TODO: add members and methods to Gotopath_point.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Gotopath_point::Create();