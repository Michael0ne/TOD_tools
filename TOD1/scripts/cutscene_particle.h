class cutscene_particle : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cutscene_particle", "Node", false, false);
		
		//	TODO: add members and methods to cutscene_particle.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cutscene_particle::Create();