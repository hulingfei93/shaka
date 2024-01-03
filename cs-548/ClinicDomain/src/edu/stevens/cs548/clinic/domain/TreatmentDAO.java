package edu.stevens.cs548.clinic.domain;

import javax.persistence.EntityManager;

public class TreatmentDAO implements ITreatmentDAO {

	public TreatmentDAO(EntityManager em) {
		this.em = em;
	}
	
	private EntityManager em;
	
	@Override
	public Treatment getTreatmentByDbId(long id) throws TreatmentException {
		Treatment treatment = em.find(Treatment.class, id);
		if(treatment == null) {
			throw new TreatmentException("Missing treatment: id = " + id);
		}
		else {
			return treatment;
		}
		
	}

	@Override
	public void addTreatment(Treatment treatment) {
		em.persist(treatment);
	}
	
	@Override
	public void deleteTreatment(Treatment treatment) {
		em.remove(treatment);
	}

}
