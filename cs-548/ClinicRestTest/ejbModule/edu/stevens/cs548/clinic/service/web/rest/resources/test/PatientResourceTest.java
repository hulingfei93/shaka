package edu.stevens.cs548.clinic.service.web.rest.resources.test;

import java.util.Date;

import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.xml.bind.DatatypeConverter;

import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientServiceException;
import edu.stevens.cs548.clinic.service.ejb.IPatientServiceRemote;
import edu.stevens.cs548.clinic.service.web.rest.PatientRepresentation;

@Stateless(name="PatientResourceTest")
public class PatientResourceTest {
    
    public PatientResourceTest() {
    }
    
    @EJB
    private IPatientServiceRemote patientService;
    

    public PatientRepresentation getPatient(String id) {
       try {
           long key = Long.parseLong(id);
           PatientDTO patientDTO = patientService.getPatientByDbId(key);
           PatientRepresentation patientRep = new PatientRepresentation(patientDTO);
           return patientRep;
       }
       catch(PatientServiceException ex) {
           throw new WebApplicationException(Response.Status.NOT_FOUND);
       }
    }
    

    public PatientRepresentation getPatientByPatientId(String patientId) {
       try {
           long pid = Long.parseLong(patientId);
           PatientDTO patientDTO = patientService.getPatientByPatientId(pid);
           PatientRepresentation patientRep = new PatientRepresentation(patientDTO);
           return patientRep;
       }
       catch(PatientNotFoundException ex) {
           throw new WebApplicationException();
       }
       catch(PatientServiceException ex) {
           throw new WebApplicationException();
       }
    }
    

    public PatientRepresentation[] getPatientByNameDob(String name, String dob) {
       Date birthDate = DatatypeConverter.parseDate(dob).getTime();
       PatientDTO[] patientDTOs = patientService.getPatientByNameDob(name, birthDate);
       PatientRepresentation[] patientReps = new PatientRepresentation[patientDTOs.length];
       for(int i = 0; i < patientDTOs.length; ++i) {
           patientReps[i] = new PatientRepresentation(patientDTOs[i]);
       }
       return patientReps;
    }
    
    public long addPatient(PatientRepresentation patientRep, int age) {
        try {
            long id = patientService.createPatient(patientRep.getName(), patientRep.getDob(), age, patientRep.getPatientId());
            return id;
        }
        catch (PatientServiceException ex) {
            throw new WebApplicationException();
        }
    }

}
