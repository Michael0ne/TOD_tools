class Rotater : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Rotater", "Node", false, false);
		
		//	TODO: add members and methods to Rotater.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Rotater::Create();