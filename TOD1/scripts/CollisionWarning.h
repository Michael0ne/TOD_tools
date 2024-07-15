class CollisionWarning : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("CollisionWarning", "Node", false, false);
		
		//	TODO: add members and methods to CollisionWarning.
		
		inst->CalculateSize();
		
		return inst;
	};
};

CollisionWarning::Create();