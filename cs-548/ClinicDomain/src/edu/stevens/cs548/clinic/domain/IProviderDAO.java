package edu.stevens.cs548.clinic.domain;

import java.util.List;

public interface IProviderDAO {
	
	public static class ProviderException extends Exception {
		private static final long serialVersionUID = 1L;
		public ProviderException(String msg) {
			super(msg);
		}
	}
	
	public Provider getProviderByNPI(long NPI) throws ProviderException;
	
	public List<Provider> getProviderByName(String name);
	
	public void addProvider(long NPI, String name, ProviderSpecEnum spec) throws ProviderException;
	
	public void deleteProvider(long NPI) throws ProviderException;

}
