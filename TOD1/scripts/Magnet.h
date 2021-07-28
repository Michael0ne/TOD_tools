class Magnet : public GlobalScript
{
public:
	static GlobalScript* Create()
	{
		static GlobalScript* inst = new GlobalScript("Magnet", "Node", false, false);
		
		inst->CalculateSize();
		return inst;
	};
};

Magnet::Create();