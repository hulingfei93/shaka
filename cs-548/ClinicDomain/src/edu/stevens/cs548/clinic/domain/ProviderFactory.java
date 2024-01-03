package edu.stevens.cs548.clinic.domain;

public class ProviderFactory implements IProviderFactory {

    @Override
    public Provider createProvider(long NPI, String name, ProviderSpecEnum spec) {
        Provider newProvider = new Provider();
        newProvider.setNPI(NPI);
        newProvider.setName(name);
        newProvider.setSpecialization(spec);
        return newProvider;
    }

}
