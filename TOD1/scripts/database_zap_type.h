class database_zap_type : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_zap_type", "Node", false, false);
		
		//	TODO: add members and methods to database_zap_type.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_zap_type::Create();