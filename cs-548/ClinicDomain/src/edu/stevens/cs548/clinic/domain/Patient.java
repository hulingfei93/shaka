package edu.stevens.cs548.clinic.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.logging.Logger;

import javax.persistence.*;

import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import static javax.persistence.CascadeType.REMOVE;

/**
 * Entity implementation class for Entity: Patient
 *
 */


@Entity
@NamedQueries({
	@NamedQuery(
			name="SearchPatientByNameDOB", 
			query="select p from Patient p where p.name = :name and p.birthDate = :dob"),
	@NamedQuery(
			name="SearchPatientByPatientId", 
			query="select p from Patient p where p.patientId = :pid")
})
@Table(name = "patient")
public class Patient implements Serializable {

	private static final long serialVersionUID = 1L;
	
	private static Logger logger = Logger.getLogger("edu.stevens.cs548.clinic.domain.Patient");
	
	@Id
	@GeneratedValue
	private long id;
	private long patientId;
	private String name;
	@Temporal(TemporalType.DATE)
	private Date birthDate;

	public Patient() {
		super();
		treatments = new ArrayList<Treatment>();
	}
	
	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public long getPatientId() {
		return patientId;
	}

	public void setPatientId(long patientId) {
		this.patientId = patientId;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Date getBirthDate() {
		return birthDate;
	}

	public void setBirthDate(Date birthDate) {
		this.birthDate = birthDate;
	}

	@OneToMany(mappedBy = "patient", cascade = REMOVE)
	@OrderBy
	private List<Treatment> treatments;
	
	protected List<Treatment> getTreatments() {
		return treatments;
	}
	
	protected void setTreatments(List<Treatment> treatments) {
		this.treatments = treatments;
	}
	
	@Transient
	private ITreatmentDAO treatmentDAO;
	
	public void setTreatmentDAO(ITreatmentDAO treatmentDAO) {
		this.treatmentDAO = treatmentDAO;
	}
	
	long addTreatment(Treatment treatment) {
		if(treatment.getPatient() != this)
			treatment.setPatient(this);
		this.treatmentDAO.addTreatment(treatment);
		this.getTreatments().add(treatment);
		
		return treatment.getId();
	}
	
	public long addDrugTreatment(String diagnosis, String drug, float dosage, Provider provider) {
		DrugTreatment drugTreatment = new DrugTreatment();
		drugTreatment.setDiagnosis(diagnosis);
		drugTreatment.setDrug(drug);
		drugTreatment.setDosage(dosage);
		drugTreatment.setProvider(provider);
		drugTreatment.getProvider().getTreatments().add(drugTreatment);
		return addTreatment(drugTreatment);
	}
	
	public long addRadiologyTreatment(String diagnosis, List<Date> dates, Provider provider) {
		RadiologyTreatment radiologyTreatment = new RadiologyTreatment();
		radiologyTreatment.setDiagnosis(diagnosis);
		if(dates != null) {
			for(Date date : dates) {
				RadDate rd = new RadDate();
				rd.setDate(date);
				radiologyTreatment.addDate(rd);
			}
		}
		radiologyTreatment.setProvider(provider);
		radiologyTreatment.getProvider().getTreatments().add(radiologyTreatment);
		return addTreatment(radiologyTreatment);
	}
	
	public long addSurgeryTreatment(String diagnosis, Date date, Provider provider) {
		SurgeryTreatment surgeryTreatment = new SurgeryTreatment();
		surgeryTreatment.setDiagnosis(diagnosis);
		surgeryTreatment.setDate(date);
		surgeryTreatment.setProvider(provider);
		surgeryTreatment.getProvider().getTreatments().add(surgeryTreatment);
		return addTreatment(surgeryTreatment);
	}
	
	public List<Long> getTreatmentIds() {
		List<Long> tids = new ArrayList<Long>();
		for(Treatment t : this.getTreatments()) {
			tids.add(t.getId());
		}
		return tids;
	}
	
	public void visitTreatment(long tid, ITreatmentVisitor visitor) throws TreatmentException {
		Treatment t = treatmentDAO.getTreatmentByDbId(tid);
		if(t.getPatient() != this) {
			logger.info("Inappropriate treatment access: patient = " + id + ", treatment = " + tid);
			throw new TreatmentException("Inappropriate treatment access: patient = " + id + ", treatment = " + tid);
		}
		t.visit(visitor);
	}
	
	public void visitTreatments(ITreatmentVisitor visitor) {
		for(Treatment t : this.getTreatments()) {
			t.visit(visitor);
		}
	}
	
	public void deleteTreatment(long tid) throws TreatmentException {
		Treatment t = treatmentDAO.getTreatmentByDbId(tid);
		if(t == null) {
			throw new TreatmentException("Delete failure: treatment: " + tid + " does not exist.");
		}
		if(t.getPatient() != this) {
			throw new TreatmentException("Inappropriate treatment access: patient = " + id + ", treatment = " + tid);
		}
		treatmentDAO.deleteTreatment(t);
		this.treatments.remove(t);
		t.getProvider().getTreatments().remove(t);
	} 
	
	
}
