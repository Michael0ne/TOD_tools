class particle_loop : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("particle_loop", "Node", false, false);
		
		//	TODO: add members and methods to particle_loop.
		
		inst->CalculateSize();
		
		return inst;
	};
};

particle_loop::Create();