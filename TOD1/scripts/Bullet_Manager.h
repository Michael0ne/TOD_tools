class Bullet_Manager : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Bullet_Manager", "Node", false, false);
		
		//	TODO: add members and methods to Bullet_Manager.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Bullet_Manager::Create();