class Magnet : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Magnet", "Node", false, false);
		
		inst->CalculateSize();
		return inst;
	};
};

Magnet::Create();