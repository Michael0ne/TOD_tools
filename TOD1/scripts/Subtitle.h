class Subtitle : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Subtitle", "Node", false, false);
		
		//	TODO: add members and methods to Subtitle.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Subtitle::Create();