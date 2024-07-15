class SpriteFader : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("SpriteFader", "Node", false, false);
		
		//	TODO: add members and methods to SpriteFader.
		
		inst->CalculateSize();
		
		return inst;
	};
};

SpriteFader::Create();