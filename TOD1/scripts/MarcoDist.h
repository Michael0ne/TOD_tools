class MarcoDist : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("MarcoDist", "Node", false, false);
		
		//	TODO: add members and methods to MarcoDist.
		
		inst->CalculateSize();
		
		return inst;
	};
};

MarcoDist::Create();