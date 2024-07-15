class generic_particle_emitter : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("generic_particle_emitter", "Node", false, false);
		
		//	TODO: add members and methods to generic_particle_emitter.
		
		inst->CalculateSize();
		
		return inst;
	};
};

generic_particle_emitter::Create();