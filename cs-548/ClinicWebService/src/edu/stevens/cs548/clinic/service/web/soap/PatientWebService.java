package edu.stevens.cs548.clinic.service.web.soap;

import java.util.Date;

import javax.ejb.EJB;
import javax.jws.WebService;
import javax.inject.Inject;

import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientServiceException;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.TreatmentNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IPatientServiceRemote;

@WebService(
        endpointInterface="edu.stevens.cs548.clinic.service.web.soap.IPatientWebService",
        targetNamespace="http://cs548.stevens.edu/clinic/service/web/soap/patient",
        serviceName="PatientWebService",
        portName="PatientWebPort")
public class PatientWebService implements IPatientWebService {

    //@Inject
    @EJB
    private IPatientServiceRemote patientService;

    @Override
    public long createPatient(String name, Date dob, int age, long patientId)
            throws PatientServiceException {
        return patientService.createPatient(name, dob, age, patientId);
    }

    @Override
    public PatientDTO getPatientByDbId(long id) throws PatientServiceException {
        return patientService.getPatientByDbId(id);
    }

    @Override
    public PatientDTO getPatientByPatientId(long pid)
            throws PatientServiceException, PatientNotFoundException {
        return patientService.getPatientByPatientId(pid);
    }   

    @Override
    public PatientDTO[] getPatientByNameDob(String name, Date dob) {
        return patientService.getPatientByNameDob(name, dob);
    }

    @Override
    public void deletePatient(String name, long id)
            throws PatientServiceException {
        patientService.deletePatient(name, id);
    }

    @Override
    public TreatmentDTO[] getTreatments(long patientId)
            throws PatientNotFoundException, PatientServiceException {
        return patientService.getTreatments(patientId);
    }

    @Override
    public void deleteTreatment(long id, long tid)
            throws PatientNotFoundException, TreatmentNotFoundException,
            PatientServiceException {
        patientService.deleteTreatment(id, tid);
    }

    @Override
    public String siteInfo() {
        return patientService.siteInfo();
    }
    
    

}
