class Particle_on_Hit : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Particle_on_Hit", "Node", false, false);
		
		//	TODO: add members and methods to Particle_on_Hit.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Particle_on_Hit::Create();