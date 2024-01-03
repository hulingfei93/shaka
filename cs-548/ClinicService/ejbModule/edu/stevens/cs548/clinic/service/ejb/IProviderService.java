package edu.stevens.cs548.clinic.service.ejb;

import java.util.Date;

import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;

public interface IProviderService {

    public class ProviderServiceException extends Exception {
        private static final long serialVersionUID = 1L;

        public ProviderServiceException(String m) {
            super(m);
        }
    }
    
    public class ProviderNotFoundException extends ProviderServiceException {
        private static final long serialVersionUID = 1L;

        public ProviderNotFoundException(String m) {
            super(m);
        }
    }
    
    public class PatientNotFoundException extends ProviderServiceException {
        private static final long serialVersionUID = 1L;

        public PatientNotFoundException(String m) {
            super(m);
        }
    }
    
    public class TreatmentNotFoundException extends ProviderServiceException {
        private static final long serialVersionUID = 1L;

        public TreatmentNotFoundException(String m) {
            super(m);
        }
    }
    
    public long createProvider(String name, long NPI) throws ProviderServiceException; 
    
    public ProviderDTO[] getProviderByName(String name);
    
    public ProviderDTO getProviderByNPI(long NPI) throws ProviderNotFoundException, ProviderServiceException;
    
    public long addTreatment(long patientDbId, long NPI, TreatmentDTO treatment) throws PatientNotFoundException, ProviderNotFoundException, ProviderServiceException;
    
    public void deleteTreatment(long tid, long patientDbId, long NPI) throws TreatmentNotFoundException, ProviderServiceException;
    
    public TreatmentDTO[] getTreatments(long patientDbId, long NPI) throws ProviderNotFoundException, ProviderServiceException;
    
    public TreatmentDTO[] getTreatments(long NPI) throws ProviderNotFoundException, ProviderServiceException;
}
