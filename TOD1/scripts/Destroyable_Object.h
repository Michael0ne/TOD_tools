class Destroyable_Object : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Destroyable_Object", "Node", false, false);
		
		//	TODO: add members and methods to Destroyable_Object.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Destroyable_Object::Create();