package edu.stevens.cs548.clinic.domain;

import java.util.List;

import javax.persistence.EntityManager;
import javax.persistence.TypedQuery;

public class ProviderDAO implements IProviderDAO {

    private EntityManager em;
    private TreatmentDAO treatmentDAO;
    
    public ProviderDAO(EntityManager em) {
        this.em = em;
        this.treatmentDAO = new TreatmentDAO(em);
    }
    
    @Override
    public Provider getProviderByNPI(long NPI) throws ProviderException {
        Provider provider = em.find(Provider.class, NPI);
        if(provider == null) {
            return null;
        }
        else {
            provider.setTreatmentDAO(this.treatmentDAO);
            return provider;
        }
    }

    @Override
    public List<Provider> getProviderByName(String name) {
        TypedQuery<Provider> query = em.createNamedQuery("SearchProviderByName", Provider.class)
                .setParameter("name", name);
        List<Provider> providers = query.getResultList();
        for(Provider provider : providers) {
            provider.setTreatmentDAO(this.treatmentDAO);
        }
        return providers;
    }

    @Override
    public void addProvider(long NPI, String name, ProviderSpecEnum spec) throws ProviderException {
        Provider provider = em.find(Provider.class, NPI);
        if(provider != null) {
            throw new ProviderException("Insertion: Provider with NPI (" 
                    + NPI
                    + ") already exists.\n Name: "
                    + provider.getName());
        }
        else {
            Provider newProvider = new Provider();
            newProvider.setNPI(NPI);
            newProvider.setName(name);
            newProvider.setSpecialization(spec);
            em.persist(newProvider);
            newProvider.setTreatmentDAO(this.treatmentDAO);
        }
    }

    @Override
    public void deleteProvider(long NPI) throws ProviderException {
        Provider provider = em.find(Provider.class, NPI);
        if(provider == null) {
            throw new ProviderException("Cannot delete provider with NPI = " + NPI + ": not found.");
        }
        else {
            em.remove(provider);
        }
    }

}
