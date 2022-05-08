class common_trigger : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("common_trigger", "Node", true, false);

		inst->CalculateSize();

		return inst;
	};
};

common_trigger::Create();