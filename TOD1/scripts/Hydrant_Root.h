class Hydrant_Root : public GlobalScript
{
public:
	static GlobalScript* Create()
	{
		static GlobalScript* inst = new GlobalScript("Hydrant_Root", "Node", false, false);

		inst->CalculateSize();

		return inst;
	}
};

Hydrant_Root::Create();