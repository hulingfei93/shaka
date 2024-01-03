package edu.stevens.cs548.clinic.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.logging.Logger;

import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.OrderBy;
import javax.persistence.Table;
import javax.persistence.Transient;

import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;

/**
 * Entity implementation class for Entity: Provider
 *
 */
@Entity
@NamedQueries({
	@NamedQuery(
			name="SearchProviderByNPI", 
			query="select p from Provider p where p.NPI = :NPI"),
	@NamedQuery(
			name="SearchProviderByName", 
			query="select p from Provider p where p.name = :name")
})
@Table(name = "provider")
public class Provider implements Serializable, ITreatmentFactory {

	private static final long serialVersionUID = 1L;
	
	private static Logger logger = Logger.getLogger("edu.stevens.cs548.clinic.domain.Provider");
	
	@Id
	private long NPI;
	private String name;
	@Enumerated(EnumType.ORDINAL)
	private ProviderSpecEnum specialization;
	
	public Provider() {
		super();
		treatments = new ArrayList<Treatment>();
	}
	
	public long getNPI() {
		return NPI;
	}

	public void setNPI(long NPI) {
		this.NPI = NPI;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public ProviderSpecEnum getSpecialization() {
		return specialization;
	}

	public void setSpecialization(ProviderSpecEnum spec) {
		this.specialization = spec;
	}
	
	
	@OneToMany(mappedBy = "provider")
	@OrderBy
	private List<Treatment> treatments;
	
	public List<Treatment> getTreatments() {
		return treatments;
	}
	
	public void setTreatments(List<Treatment> treatments) {
		this.treatments = treatments;
	}
	
	@Transient
	private ITreatmentDAO treatmentDAO;
	
	public void setTreatmentDAO(ITreatmentDAO treatmentDAO) {
		this.treatmentDAO = treatmentDAO;
	}
	
	public List<Long> getTreatmentIds() {
		List<Long> tids = new ArrayList<Long>();
		for(Treatment t : this.getTreatments()) {
			tids.add(t.getId());
		}
		return tids;
	}
	
	public List<Long> getTreatmentIds(Patient patient) {
		List<Long> tids = new ArrayList<Long>();
		for(Treatment t : this.getTreatments()) {
			if(t.getPatient().getId() == patient.getId()) {
				tids.add(t.getId());
			}
		}
		return tids;
	}
	
	long addTreatment(Treatment treatment) {
		if(treatment.getProvider() != this)
			treatment.setProvider(this);
		this.treatmentDAO.addTreatment(treatment);
		this.getTreatments().add(treatment);
		
		return treatment.getId();
	}
	
	public long addDrugTreatment(String diagnosis, String drug, float dosage, Patient patient) {
		DrugTreatment drugTreatment = new DrugTreatment();
		drugTreatment.setDiagnosis(diagnosis);
		drugTreatment.setDrug(drug);
		drugTreatment.setDosage(dosage);
		drugTreatment.setPatient(patient);
		drugTreatment.getPatient().getTreatments().add(drugTreatment);
		return addTreatment(drugTreatment);
	}
	
	public long addRadiologyTreatment(String diagnosis, List<Date> dates, Patient patient) {
		RadiologyTreatment radiologyTreatment = new RadiologyTreatment();
		radiologyTreatment.setDiagnosis(diagnosis);
		if(dates != null) {
			for(Date date : dates) {
				RadDate rd = new RadDate();
				rd.setDate(date);
				radiologyTreatment.addDate(rd);
			}
		}
		radiologyTreatment.setPatient(patient);
		radiologyTreatment.getPatient().getTreatments().add(radiologyTreatment);
		return addTreatment(radiologyTreatment);
	}
	
	public long addSurgeryTreatment(String diagnosis, Date date, Patient patient) {
		SurgeryTreatment surgeryTreatment = new SurgeryTreatment();
		surgeryTreatment.setDiagnosis(diagnosis);
		surgeryTreatment.setDate(date);
		surgeryTreatment.setPatient(patient);
		surgeryTreatment.getPatient().getTreatments().add(surgeryTreatment);
		return addTreatment(surgeryTreatment);
	}
	
	
	public void visitTreatment(long tid, ITreatmentVisitor visitor) throws TreatmentException {
		Treatment t = treatmentDAO.getTreatmentByDbId(tid);
		if(t.getProvider().getNPI() != NPI) {
			logger.info("Inappropriate treatment access: NPI = " + NPI + ", treatment = " + tid);
			throw new TreatmentException("Inappropriate treatment access: NPI = " + NPI + ", treatment = " + tid);
		}
		t.visit(visitor);
	}
	
	public void deleteTreatment(long tid) throws TreatmentException {
		Treatment t = treatmentDAO.getTreatmentByDbId(tid);
		if(t == null) {
			throw new TreatmentException("Delete failure: treatment: " + tid + " does not exist.");
		}
		if(t.getProvider() != this) {
			throw new TreatmentException("Inappropriate treatment access: provider = " + NPI + ", treatment = " + tid);
		}
		treatmentDAO.deleteTreatment(t);
		this.treatments.remove(t);
		t.getPatient().getTreatments().remove(t);
	}

	@Override
	public DrugTreatment createDrugTreatment(String diagnosis, String drug,
			float dosage, Patient patient) {
		DrugTreatment drugTreatment = new DrugTreatment();
		drugTreatment.setDiagnosis(diagnosis);
		drugTreatment.setDrug(drug);
		drugTreatment.setDosage(dosage);
		drugTreatment.setPatient(patient);
		drugTreatment.setProvider(this);
		return drugTreatment;
	}

	@Override
	public RadiologyTreatment createRadiologyTreatment(String diagnosis,
			List<Date> dates, Patient patient) {
		RadiologyTreatment radiologyTreatment = new RadiologyTreatment();
		radiologyTreatment.setDiagnosis(diagnosis);
		if(dates != null) {
			for(Date date : dates) {
				RadDate rd = new RadDate();
				rd.setDate(date);
				radiologyTreatment.addDate(rd);
			}
		}
		radiologyTreatment.setPatient(patient);
		radiologyTreatment.setProvider(this);
		return radiologyTreatment;
	}

	@Override
	public SurgeryTreatment createSurgeryTreatment(String diagnosis, Date date,
			Patient patient) {
		SurgeryTreatment surgeryTreatment = new SurgeryTreatment();
		surgeryTreatment.setDiagnosis(diagnosis);
		surgeryTreatment.setDate(date);
		surgeryTreatment.setPatient(patient);
		surgeryTreatment.setProvider(this);
		return surgeryTreatment;
	}
}


