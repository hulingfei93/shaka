package edu.stevens.cs548.clinic.domain;

public interface IProviderFactory {

	public Provider createProvider(long NPI, String name, ProviderSpecEnum spec);
}
