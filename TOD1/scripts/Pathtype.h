class Pathtype : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Pathtype", "Node", false, false);

		inst->AddStructElement(GetScriptIdByFullName("PathType:integer"), "0", NULL);

		inst->CalculateSize();
		return inst;
	};
};

Pathtype::Create();