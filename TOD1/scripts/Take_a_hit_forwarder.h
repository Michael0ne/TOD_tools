class Take_a_hit_forwarder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Take_a_hit_forwarder", "Node", false, false);
		
		//	TODO: add members and methods to Take_a_hit_forwarder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Take_a_hit_forwarder::Create();