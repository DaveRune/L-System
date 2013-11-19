class production_rules {

	public:

	void Iterate (string axiom, int iterations) {
		static string workingVariable = axiom;
		static string newVariable;

		printf("%s\n", axiom.c_str());

		for(int i = 0; i != iterations; ++i) {
			for(int v = 0; v != workingVariable.length(); ++v) {

				switch (workingVariable[v])
				{
					case 'F':
						newVariable.append("F+F-F-F+F");
						break;

					default:
						newVariable += workingVariable[v];
				}
			}
			workingVariable = newVariable;
			if(i != iterations-1)
				newVariable.clear();
		}
		printf("\n%s\n\n\n\n", newVariable.c_str());
	}
};