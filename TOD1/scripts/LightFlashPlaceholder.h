class LightFlashPlaceholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("LightFlashPlaceholder", "Node", false, false);
		
		//	TODO: add members and methods to LightFlashPlaceholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

LightFlashPlaceholder::Create();