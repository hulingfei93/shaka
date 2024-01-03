package edu.stevens.cs548.clinic.service.ejb;

import java.util.Date;
import java.util.List;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import javax.ejb.Stateless;
import javax.inject.Inject;
import javax.inject.Named;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import edu.stevens.cs548.clinic.domain.IPatientDAO;
import edu.stevens.cs548.clinic.domain.IPatientDAO.PatientException;
import edu.stevens.cs548.clinic.domain.IPatientFactory;
import edu.stevens.cs548.clinic.domain.IProviderDAO;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import edu.stevens.cs548.clinic.domain.Patient;
import edu.stevens.cs548.clinic.domain.PatientDAO;
import edu.stevens.cs548.clinic.domain.PatientFactory;
import edu.stevens.cs548.clinic.domain.ProviderDAO;
import edu.stevens.cs548.clinic.service.dto.ObjectFactory;
import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.ClinicDomainProducer.ClinicDomain;

/**
 * Session Bean implementation class PatientService
 */
@Stateless(name="PatientServiceBean")
public class PatientService implements IPatientServiceLocal, IPatientServiceRemote {

    ObjectFactory DTOObjectFactory;
    private IPatientFactory patientFactory;
    private IPatientDAO patientDAO;
    private IProviderDAO providerDAO;
    
     /**
     * Default constructor. 
     */
    public PatientService() {
        patientFactory = new PatientFactory();
        DTOObjectFactory = new ObjectFactory();
    }

    @PersistenceContext(unitName="ClinicDomain")
    //@Inject
    //@ClinicDomain
    private EntityManager em;
    
    @PostConstruct
    private void initialize() {
        patientDAO = new PatientDAO(em);
        providerDAO = new ProviderDAO(em);
    }
    
    private PatientDTO convertPatientPDOToDTO(Patient patient) {
        PatientDTO patientDTO = DTOObjectFactory.createPatientDTO();
        patientDTO.setId(patient.getId());
        patientDTO.setPatientId(patient.getPatientId());
        patientDTO.setName(patient.getName());
        patientDTO.setDob(patient.getBirthDate());
        List<Long> tids = patientDTO.getTreatments();
        for(long tid : patient.getTreatmentIds()) {
            tids.add(tid);
        }
        return patientDTO;
    }
    
    /**
     * @see IPatientService#getPatientByDbId(long)
     */
    public PatientDTO getPatientByDbId(long id) throws PatientServiceException {
        try {
            Patient patient = patientDAO.getPatientByDbId(id);
            return convertPatientPDOToDTO(patient);
        }
        catch(PatientException ex) {
            throw new PatientServiceException(ex.toString());
        }
    }

    /**
     * @see IPatientService#getPatientByPatientId(long)
     */
    public PatientDTO getPatientByPatientId(long pid) throws PatientServiceException, PatientNotFoundException {
        try {
            Patient patient = patientDAO.getPatientByPatientId(pid);
            if(patient == null)
                throw new PatientNotFoundException("Patient not found patient id = " + pid);
            return convertPatientPDOToDTO(patient);
        }
        catch(PatientException ex) {
            throw new PatientServiceException(ex.toString());
        }
    }
    
    /**
     * @see IPatientService#getPatientByNameDob(String, Date)
     */
    public PatientDTO[] getPatientByNameDob(String name, Date dob) {
        List<Patient> patients = patientDAO.getPatientByNameDob(name, dob);
        PatientDTO[] patientDTOs = new PatientDTO[patients.size()];
        for(int i = 0; i < patientDTOs.length; ++i) {
            patientDTOs[i] = convertPatientPDOToDTO(patients.get(i));
        }
        return patientDTOs;
    }

    /**
     * @see IPatientService#deletePatient(String, long)
     */
    public void deletePatient(String name, long id) throws PatientServiceException {
        try {
            Patient patient = patientDAO.getPatientByDbId(id);
            if(!patient.getName().equals(name)) {
                throw new PatientServiceException("Tried to delete wrong patient: name = " + name + " , id = " + id);
            }
            else {
                patientDAO.deletePatient(id);
            }
        }
        catch(PatientException ex) {
            throw new PatientServiceException(ex.toString());
        }
    }
    
    /**
     * @see IPatientService#createPatient(String, Date, long)
     */
    public long createPatient(String name, Date dob, int age, long patientId)  throws PatientServiceException {
        // Patient patient = patientFactory.createPatient(patientId, name, dob);
        try {
            return patientDAO.addPatient(name, dob, age, patientId);
        }
        catch(PatientException ex) {
            throw new PatientServiceException(ex.toString());
        }
    }

    /*
    @Override
    public void addDrugTreatment(long patientDbId, long providerNPI, String diagnosis, String drug,
            float dosage) throws PatientNotFoundException {
        try {
            Patient patient = patientDAO.getPatientByDbId(patientDbId);
            Provider provider = providerDAO.getProviderByNPI(providerNPI);
            patient.addDrugTreatment(diagnosis, drug, dosage, provider);
        }
        catch(PatientException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
        catch(ProviderException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
    }*/

    /*
    @Override
    public TreatmentDTO[] getTreatments(long id, long[] tids)
            throws PatientNotFoundException, TreatmentNotFoundException,
            PatientServiceException {
        try {
            Patient patient = patientDAO.getPatientByDbId(id);
            TreatmentDTO[] treatmentDTOs = new TreatmentDTO[tids.length];
            for(int i = 0; i < tids.length; ++i) {
                TreatmentPDOToDTO visitor = new TreatmentPDOToDTO();
                patient.visitTreatment(tids[i], visitor);
                treatmentDTOs[i] = visitor.getTreatmentDTO();
            }
            return treatmentDTOs;
        }
        catch(PatientException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
        catch(TreatmentException ex) {
            throw new PatientServiceException(ex.toString());
        }
    }*/
    
    public TreatmentDTO[] getTreatments(long patientId) throws PatientNotFoundException, PatientServiceException {
        try {
            Patient patient = patientDAO.getPatientByPatientId(patientId);
            if(patient == null)
                throw new PatientNotFoundException("Patient not found patient id = " + patientId);
            List<Long> tids = patient.getTreatmentIds();
            TreatmentDTO[] treatmentDTOs = new TreatmentDTO[tids.size()];
            for(int i = 0; i < tids.size(); ++i) {
                TreatmentPDOToDTO visitor = new TreatmentPDOToDTO();
                patient.visitTreatment(tids.get(i), visitor);
                treatmentDTOs[i] = visitor.getTreatmentDTO();
            }
            return treatmentDTOs;
        }
        catch(PatientException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
        catch(TreatmentException ex) {
            throw new PatientServiceException(ex.toString());
        }
    }

    @Override
    public void deleteTreatment(long id, long tid)
            throws PatientNotFoundException, TreatmentNotFoundException,
            PatientServiceException {
        try {
            Patient patient = patientDAO.getPatientByDbId(id);
            patient.deleteTreatment(tid);
        }
        catch(PatientException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
        catch(TreatmentException ex) {
            throw new PatientServiceException(ex.toString());
        }
        
    }

    @Resource(name="SiteInfo")
    private String siteInformation;
    
    @Override
    public String siteInfo() {
        return siteInformation;
    }

    

}
