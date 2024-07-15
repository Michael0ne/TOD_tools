class Trigger_Activate_Particle : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Activate_Particle", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Activate_Particle.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Activate_Particle::Create();