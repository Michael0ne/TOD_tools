class Rotater : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Rotater", "Node", false, false);

		inst->CalculateSize();
		return inst;
	};
};

Rotater::Create();