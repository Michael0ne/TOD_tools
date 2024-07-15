class node_collision_damage : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("node_collision_damage", "Node", false, false);
		
		//	TODO: add members and methods to node_collision_damage.
		
		inst->CalculateSize();
		
		return inst;
	};
};

node_collision_damage::Create();