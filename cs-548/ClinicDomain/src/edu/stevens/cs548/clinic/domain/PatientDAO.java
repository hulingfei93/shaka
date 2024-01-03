package edu.stevens.cs548.clinic.domain;

import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.logging.Logger;

import javax.persistence.EntityManager;
import javax.persistence.TypedQuery;

import edu.stevens.cs548.clinic.domain.IPatientDAO.PatientException;

public class PatientDAO implements IPatientDAO {
    
    private EntityManager em;
    private TreatmentDAO treatmentDAO;
    private static Logger logger = Logger.getLogger("edu.stevens.cs548.clinic.domain.IPatientDAO");
    
    public PatientDAO(EntityManager em) {
        this.em = em;
        this.treatmentDAO = new TreatmentDAO(em);
    }
    
    @Override
    public Patient getPatientByDbId(long id) throws PatientException {
        Patient patient = em.find(Patient.class, id);
        if(patient == null) {
            throw new PatientException("Patient not found primay key = " + id);
        }
        else {
            patient.setTreatmentDAO(this.treatmentDAO);
            return patient;
        }
    }
    
    @Override
    public Patient getPatientByPatientId(long pid)
            throws PatientException {
        TypedQuery<Patient> query = em.createNamedQuery("SearchPatientByPatientId", Patient.class)
                .setParameter("pid", pid);
        List<Patient> patients = query.getResultList();
        if(patients.size() > 1) {
            logger.info("Duplicate patient records: patient id = " + pid);
            throw new PatientException("Duplicate patient records: patient id = " + pid);
        }
        else if(patients.size() < 1)
            return null;
        else {
            Patient patient = patients.get(0);
            patient.setTreatmentDAO(this.treatmentDAO);
            return patient;
        }
    }


    @Override
    public List<Patient> getPatientByNameDob(String name, Date dob) {
        TypedQuery<Patient> query = em.createNamedQuery("SearchPatientByNameDOB", Patient.class)
                .setParameter("name",  name)
                .setParameter("dob", dob);
        List<Patient> patients = query.getResultList();
        for(Patient patient : patients) {
            patient.setTreatmentDAO(this.treatmentDAO);
        }
        return patients;
    }

    private long addPatient(Patient patient) throws PatientException {
        long pid = patient.getPatientId();
        if(pid != -1) {
            TypedQuery<Patient> query = em.createNamedQuery("SearchPatientByPatientId", Patient.class)
                    .setParameter("pid", pid);
            List<Patient> patients = query.getResultList();
            if(patients.size() < 1) {
                em.persist(patient);
                patient.setTreatmentDAO(this.treatmentDAO);
            }
            else {
                Patient patient2 = patients.get(0);
                throw new PatientException("Insertion: Patient with patient id (" 
                        + pid
                        + ") already exists.\n Name: "
                        + patient2.getName());
            }
        }
        else {
            em.persist(patient);
            patient.setTreatmentDAO(this.treatmentDAO);
        }
        return patient.getId();
    }
    
    @Override
    public long addPatient(String name, Date dob, int age)
            throws PatientException {
        long pid = -1;
        return addPatient(name, dob, age, pid);
    }

    @Override
    public long addPatient(String name, Date dob, int age, long pid)
            throws PatientException {
        Calendar c = Calendar.getInstance();
        c.setTimeInMillis(dob.getTime());
        c.add(Calendar.YEAR, age);
        Calendar currentDate = Calendar.getInstance();
        
        if(c.get(Calendar.YEAR) != currentDate.get(Calendar.YEAR)) {
            throw new PatientException("birthday: " + dob.toString() + " and age: " + age + " are unmatched.");
        }
        
        Patient newPatient = new Patient();
        newPatient.setName(name);
        newPatient.setBirthDate(dob);
        newPatient.setPatientId(pid);
    
        return addPatient(newPatient);
    }

    @Override
    public void deletePatient(long id) throws PatientException {
        Patient patient = em.find(Patient.class, id);
        if(patient == null) {
            throw new PatientException("Cannot delete patient with primay key = " + id + ": not found.");
        }
        else {
            em.remove(patient);
        }
    }   
}
